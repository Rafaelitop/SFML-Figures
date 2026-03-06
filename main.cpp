#include "imgui/imconfig.h"
#include "imgui.h"
#include <imgui-SFML.h>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <fstream>
#include <sstream>
#include <vector>
#include <memory>


//Classes

class Shape
{
protected:
	sf::Text text;
	sf::Vector2f speed;
	std::string name;
	bool visible = true;

public:
	Shape(const sf::Text& text,
		const sf::Vector2f& speed,
		const std::string& name)
		: text(text), speed(speed), name(name)
	{
	}

	virtual ~Shape() = default;

	virtual void draw(sf::RenderWindow& window) = 0;
	virtual void movement(const sf::RenderWindow& window) = 0;
	virtual const sf::Color& getColor() const = 0;
	virtual void setColor(sf::Color& color) = 0;


	void setText(char newText[255])
	{
		text.setString(newText);
		
	}

	sf::Text& getText() { return text; }
	const sf::Text& getText() const { return text; }
	const sf::Vector2f& getSpeed() const { return speed; }
	const std::string& getName() const { return name; }
	void setSpeed(const sf::Vector2f& newSpeed) { speed = newSpeed; }
	void setVisible(bool v) { visible = v; }
	bool isVisible() const { return visible; }


};


class Circle : public Shape
{


	sf::CircleShape circle;
	float oRadius = 0;



public:



	Circle(float radius,
		int r, int g, int b,
		const sf::Vector2f& position,
		const sf::Vector2f& speed,
		const sf::Text& text,
		const std::string& name)
		: Shape(text, speed, name), oRadius(radius)
	{
		circle.setRadius(radius);
		circle.setFillColor(sf::Color(r, g, b));
		circle.setPosition(position);
	}


	void textCenter()
	{

		sf::FloatRect textBounds = text.getLocalBounds();
		sf::Vector2f scale = text.getScale(); 

		
	
		float x = circle.getPosition().x + circle.getRadius() - (textBounds.size.x * scale.x/2.0f);
		float y = circle.getPosition().y + circle.getRadius() - (textBounds.size.y * scale.y/ 2.0f);

		
		text.setPosition(sf::Vector2f(x,y));
	}

	void draw(sf::RenderWindow& window) override
	{
		if (!visible) return;
		window.draw(circle);
		window.draw(text);


	}

	void movement(const sf::RenderWindow& window)
	{
		sf::Vector2f currentPos = circle.getPosition();

		if (currentPos.x <0 || currentPos.x + circle.getRadius() * 2> window.getSize().x)
		{
			speed.x = -speed.x;

		}

		if (currentPos.y < 0 || currentPos.y + circle.getRadius() * 2 > window.getSize().y)
		{
			speed.y = -speed.y;
		}

		circle.setPosition(currentPos + speed);
		textCenter();

	}


	void setScale(float scale)
	{

		circle.setRadius(oRadius * scale);
		text.setScale(sf::Vector2f(1 * scale, 1 * scale));

	}


	const sf::Color& getColor() const  override
	{
		return circle.getFillColor();
	}

	void setColor(sf::Color& color) override
	{
		circle.setFillColor(color);
	}




	
	

		
};

class Rectangle : public Shape
{


	sf::RectangleShape rect;
	sf::Vector2f oSize;



public:

	Rectangle(const sf::Vector2f& size,const int r,const int g, const int b,const sf::Vector2f position,const sf::Vector2f& speed,sf::Text& text,const std::string& name)
		:Shape(text,speed,name),oSize(size)
	{
		
		rect.setPosition(position);
		rect.setFillColor(sf::Color(r, g, b));
		rect.setSize(size);


	};

	void textCenter()
	{

		sf::FloatRect textBounds = text.getLocalBounds();
		sf::Vector2f scale = text.getScale();
		float width = textBounds.size.x;
		float height = textBounds.size.y;

		sf::Vector2f textpos(rect.getPosition().x + rect.getSize().x/2 - width *scale.x / 2, rect.getPosition().y + rect.getSize().y/2 - height *scale.y / 2);
		text.setPosition(textpos);
	}

	void draw(sf::RenderWindow& window) override
	{
		if (!visible) return;
		window.draw(rect);
		window.draw(text);

	}

	void movement(const sf::RenderWindow& window)
	{
		sf::Vector2f currentPos = rect.getPosition();

		if (currentPos.x <0 || currentPos.x + rect.getSize().x > window.getSize().x)
		{
			speed.x = -speed.x;

		}

		if (currentPos.y < 0 || currentPos.y + rect.getSize().y > window.getSize().y)
		{
			speed.y = -speed.y;
		}

		
		rect.setPosition(currentPos + speed);
		textCenter();

	}

	void setScale(float scale)
	{

		rect.setSize(oSize * scale);
		text.setScale(sf::Vector2f(1 * scale, 1 * scale));
		

	}

	const sf::Color& getColor() const override
	{
		return rect.getFillColor();
	}

    void setColor(sf::Color& color) override
	{
		rect.setFillColor(color);
	}



};

//Main

int main()
{
	
	sf::RenderWindow window;
	bool open = true;
	bool checkbox = true;
	int lastItemIndex = -1;
	float initialValSlider = 1.0f;
	float v2[2] = {0.0f,0.0f};
	float sliderMinVal = 1;
	float sliderMaxval = 5;
	int itemIndex = 0;
	char buff[255] = "";
	ImVec4 color = ImVec4(1, 0, 0, 1);
	int w, h,fontSize,r,g,b;
	float radius, postionX, positionY, speedX, speedY;
	sf::Font font;
	sf::Text text(font);
	std::string line, filePath,shapeName;
	std::ifstream file("prompt.txt");
	std::vector<const char*> items;
	std::vector<std::unique_ptr<Shape>> shapes;
	
	



	if (!file.is_open())
	{

		std::cout << "Error al abrir el achivo";
	}


	while (std::getline(file,line)) 
	{
		std::stringstream ss(line);

		std::string header;

		ss >> header;

		

		if (header == "Window")
		{

			ss >> w >> h;

			
			window.create(sf::VideoMode({ static_cast<unsigned int>(w), static_cast<unsigned int>(h) }), "Title");
			window.setFramerateLimit(60);

			
			ImGui::SFML::Init(window);

	

			ImGuiStyle& style = ImGui::GetStyle();
			ImGui::StyleColorsDark();

			style.WindowRounding = 8;
			style.FrameRounding = 8;
			
		}
		else if (header == "Font") 
		{

			ss >> filePath >> fontSize >> r>>g>>b;

			if (!font.openFromFile(filePath))
			{
				std::cout << "Font dont found" << std::endl;

			}
			else
			{
				std::cout << "Font loaded" << std::endl;

			}

			text.setCharacterSize(fontSize);
			text.setFillColor(sf::Color(r, g, b));

		}
		else if (header == "Circle")
		{
			
			

			ss >> shapeName >> postionX >> positionY >>speedX >> speedY >> r >>g>>b>>radius;
			sf::Vector2f position(postionX, positionY);
			sf::Vector2f speed(speedX, speedY);
			text.setString(shapeName);
			
			shapes.push_back(
				std::make_unique<Circle>(
					radius, r, g, b,
					position,
					speed,
					text,
					shapeName
				)
			);


		}
		else if (header == "Rectangle")
		{	
		
			
			float width, height;
			ss >> shapeName >> postionX >> positionY >> speedX >> speedY >> r >> g >> b >> width >> height;

			sf::Vector2f size(width, height);
			sf::Vector2f speed(speedX, speedY);
			sf::Vector2f position(postionX, positionY);

			text.setString(shapeName);
			shapes.push_back(
				std::make_unique<Rectangle>(
					size, r, g, b,
					position,
					speed,
					text,
					shapeName
				)
			);

		}

	}
	


	sf::Clock deltaClock;
	while (window.isOpen())
	{	
		while (const std::optional event = window.pollEvent())
		{
			ImGui::SFML::ProcessEvent(window,*event);
			if (event->is<sf::Event::Closed>())
			{
				window.close();
			}


		}
		ImGui::SFML::Update(window, deltaClock.restart());

		window.clear();


	

		sf::Vector2 windowBoundaries = window.getSize();
		ImGui::SetNextWindowSize(ImVec2(windowBoundaries.x/3,windowBoundaries.y/3));
	
	
			ImGui::Begin("Shape properties",&open);


			items.clear();
			for (auto& s : shapes) 
			{
				items.push_back(s->getName().c_str());
			
			}


			
	
			ImGui::Dummy(ImVec2(0,5));
			ImGui::Combo("Shape",&itemIndex,items.data(),(int)items.size());
			ImGui::Dummy(ImVec2(0, 5));
			ImGui::Checkbox("Draw Shape", &checkbox);
			ImGui::Dummy(ImVec2(0, 5));
			ImGui::SliderFloat("Scale", &initialValSlider, sliderMinVal, sliderMaxval, "%.2f");
			
			


			if (!shapes.empty() && itemIndex < shapes.size())
			{
			
				sf::Vector2f newSpeed(v2[0],v2[1]);
				Shape& selected = *shapes[itemIndex];


	

				if (itemIndex != lastItemIndex || buff[0] == '\0')
				{
					v2[0] = selected.getSpeed().x;
					v2[1] = selected.getSpeed().y;

					sf::Color c = selected.getColor();

				

					


					color = ImVec4(
						c.r / 255.f,
						c.g / 255.f,
						c.b / 255.f,
						c.a / 255.f
					);	


					std::string text = selected.getText().getString().toAnsiString();

					strncpy_s(buff, sizeof(buff) ,text.c_str(),_TRUNCATE);

					

					lastItemIndex = itemIndex;

					

				}

				bool visible = selected.isVisible();

				selected.setVisible(checkbox);

				if (Circle* c = dynamic_cast<Circle*>(&selected))
				{

					c->setScale(initialValSlider);
				

				}
				else if (Rectangle* r = dynamic_cast<Rectangle*>(&selected))
				{
					r->setScale(initialValSlider);
					

				}

				

			
				
			}

			if (ImGui::SliderFloat2("velocity", v2, -10, 10))
			{
				if (!shapes.empty() && itemIndex < shapes.size())
				{
					Shape& selected = *shapes[itemIndex];
					selected.setSpeed({ v2[0], v2[1] });
				}
			}

		
			
			if (ImGui::ColorEdit4("Color", (float*)&color)) 
			{

				if (!shapes.empty() && itemIndex < shapes.size())
				{

					Shape& selected = *shapes[itemIndex];

					sf::Color newColor(

						color.x * 255,
						color.y * 255,
						color.z * 255,
						color.w * 255

					);

					selected.setColor(newColor);
				}
			
			}
		
			if (ImGui::InputText("Name", buff, sizeof(buff)))
			{
				if (!shapes.empty() && itemIndex < shapes.size())
				{
					Shape& selected = *shapes[itemIndex];
					

					selected.getText().setString(buff);

					selected.setText(buff);
					
				}
			}


		
		ImGui::End();

		ImGui::SFML::Render(window);



		for (auto& s : shapes)
		{

	
			s->draw(window);
			s->movement(window);

		}


		window.display();

	}
	

}